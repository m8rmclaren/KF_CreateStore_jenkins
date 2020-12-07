pipeline {
    agent any
    parameters {
        string(name: 'StoreName', defaultValue: 'blogStoreTest.jks')
        string(name: 'CertType', defaultValue: 'JKS')
        string(name: 'Path', defaultValue: '/home/hroszell/certificates/')
        string(name: 'ContainerID', defaultValue: '1')
        string(name: 'Inventory', defaultValue: '1')
    }
    stages {
        stage('Build project') {
            steps {
                echo "Compiling project"
                withEnv(['PATH+EXTRA=/usr/sbin:/usr/bin:/sbin:/bin']) {
                    sh '''gcc project.c'''
                }
            }
        }
        stage('Request new cert store') {
            steps {
                echo "Calling KeyFactor"
                withEnv(['PATH+EXTRA=/usr/sbin:/usr/bin:/sbin:/bin']) {
                    sh "python3 main.py ${params.StoreName} ${params.CertType} ${params.Path} ${params.ContainerID} ${params.Inventory}"
                }
            }
        }
        stage('Run project') {
            steps {
                echo "Running project"
                withEnv(['PATH+EXTRA=/usr/sbin:/usr/bin:/sbin:/bin']) {
                    sh '''./a.out < input.txt'''
                }
            }
        }
    }
}